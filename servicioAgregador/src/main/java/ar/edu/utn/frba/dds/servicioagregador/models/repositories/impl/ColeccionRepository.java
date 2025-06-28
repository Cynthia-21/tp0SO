package ar.edu.utn.frba.dds.servicioagregador.models.repositories.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Coleccion;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes.Fuente;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IColeccionRepository;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.UUID;
import java.util.stream.Collectors;

@Repository
public class ColeccionRepository implements IColeccionRepository {
  private List<Coleccion> colecciones;

  public ColeccionRepository() {
    this.colecciones = new ArrayList<>();
  }

  @Override
  public Coleccion save(Coleccion coleccion) {
    if (coleccion.getId() == null) {
      coleccion.setId(this.obtenerProximaId());
      this.colecciones.add(coleccion);
    } else {
      this.colecciones.remove(this.findById(coleccion.getId()));
      this.colecciones.add(coleccion);
    }
    return coleccion;
  }

  @Override
  public List<Coleccion> findAll() {
    return this.colecciones;
  }

  @Override
  public Coleccion findById(String id) {
    return this.colecciones.stream().filter(coleccion -> coleccion.getId().equals(id)).findFirst().orElse(null);
    //Usamos findFirst porque no tenemos la consistencia de la base de datos. tomamos el primero
  }

  @Override
  public List<Coleccion> findByFuente(Fuente fuente) {
    return colecciones.stream()
        .filter(coleccion -> coleccion.getFuentes().contains(fuente))
        .collect(Collectors.toList());
  }

  @Override
  public List<Coleccion> findByCambioCriterio() {
    return this.colecciones.stream()
        .filter(coleccion -> coleccion.getCriterioDePertenencia().getFueModificado())
        .collect(Collectors.toList());
  }

  @Override
  public void delete(Coleccion coleccion) {
    this.colecciones.remove(coleccion);
  }
  
  private String obtenerProximaId() {
    return UUID.randomUUID().toString().replace("-", "");
  }
}
