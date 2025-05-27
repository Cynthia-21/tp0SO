package ar.edu.utn.frba.dds.fuentedinamica.models.repositories;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.solicitudes.SolicitudDeModificacion;

import java.util.List;

public interface ISolicitudDeModificacionRepository {
  public SolicitudDeModificacion findById(Long id);
  public SolicitudDeModificacion save(SolicitudDeModificacion solicitud);
  public List<SolicitudDeModificacion> findAll();
}
