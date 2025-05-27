package ar.edu.utn.frba.dds.fuentedinamica.models.repositories.impl;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes.SolicitudDeModificacion;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.ISolicitudDeModificacionRepository;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;

@Repository
public class SolicitudDeModificacionRepository implements ISolicitudDeModificacionRepository {
  private List<SolicitudDeModificacion> solicitudes;

  public SolicitudDeModificacionRepository(){
    this.solicitudes = new ArrayList<>();
  }


  @Override
  public SolicitudDeModificacion save(SolicitudDeModificacion solicitud) {
    solicitud.setId((long) this.solicitudes.size());
    this.solicitudes.add(solicitud);

    return solicitud;
  }

  @Override
  public List<SolicitudDeModificacion> findAll() {
    return this.solicitudes;
  }

  @Override
  public SolicitudDeModificacion findById(Long id) {
    return this.solicitudes.stream().filter(solicitud -> solicitud.getId().equals(id)).findFirst().orElse(null);
  }

}
