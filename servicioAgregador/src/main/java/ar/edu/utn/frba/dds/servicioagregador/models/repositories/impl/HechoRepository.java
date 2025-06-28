package ar.edu.utn.frba.dds.servicioagregador.models.repositories.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IHechoRepository;
import org.springframework.stereotype.Repository;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Repository
public class HechoRepository implements IHechoRepository {

  private List<Hecho> hechos;

  public HechoRepository() {
    this.hechos = new ArrayList<>();
  }

  private Long obtenerProximaId() {
    return ((long) this.hechos.size() + 1);
  }

  @Override
  public Hecho save(Hecho hecho) {
    if (hecho.getId() == null) {
      hecho.setId(this.obtenerProximaId());
      this.hechos.add(hecho);
    } else {
      this.hechos.remove(this.findById(hecho.getId()));
      this.hechos.add(hecho);
    }
    return hecho;
  }

  @Override
  public List<Hecho> saveVarios(List<Hecho> hechos) {
    List<Hecho> guardados = new ArrayList<>();
    for (Hecho hecho : hechos) {
      if (hecho.getId() == null) {
        hecho.setId(this.obtenerProximaId());
        this.hechos.add(hecho);
      } else {
        this.hechos.remove(this.findById(hecho.getId()));
        this.hechos.add(hecho);
      }
      guardados.add(hecho);
    }
    return guardados;
  }

  @Override
  public List<Hecho> findAll() {
    return this.hechos;
  }

  @Override
  public Hecho findById(Long id) {
    return this.hechos.stream().filter(hecho -> hecho.getId().equals(id)).findFirst().orElse(null);
  }

}
